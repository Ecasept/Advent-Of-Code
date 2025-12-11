use proc_macro::TokenStream;
use quote::quote;
use syn::{parse::Parse, parse_macro_input, ItemFn, LitInt, Token};

struct PartArgs {
    day: u8,
    part: u8,
}

impl Parse for PartArgs {
    fn parse(input: syn::parse::ParseStream) -> syn::Result<Self> {
        let day: LitInt = input.parse()?;
        input.parse::<Token![,]>()?;
        let part: LitInt = input.parse()?;
        
        Ok(PartArgs {
            day: day.base10_parse()?,
            part: part.base10_parse()?,
        })
    }
}

#[proc_macro_attribute]
pub fn aoc(attr: TokenStream, item: TokenStream) -> TokenStream {
    let args = parse_macro_input!(attr as PartArgs);
    let day_num = args.day;
    let part_num = args.part;
    
    let func = parse_macro_input!(item as ItemFn);
    let func_name = &func.sig.ident;
    let wrapper_name = quote::format_ident!("{}_wrapper", func_name);
    
    let expanded = quote! {
        #func
        
        fn #wrapper_name() -> Result<String, String> {
            match #func_name() {
                Ok(val) => Ok(val.to_string()),
                Err(e) => Err(e.to_string()),
            }
        }
        
        inventory::submit! {
            crate::days::Solution {
                day: #day_num,
                part: #part_num,
                func: #wrapper_name,
            }
        }
    };
    
    TokenStream::from(expanded)
}